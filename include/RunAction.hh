#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "globals.hh"
#include <vector>
#include <string>

// ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "G4Threading.hh"
#include <mutex>
#include <atomic>
#include <condition_variable>

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    // Access methods for ROOT objects
    TFile* GetRootFile() const { return fRootFile; }
    TTree* GetTree() const { return fTree; }
    
    // Thread synchronization for ROOT file operations
    static void WaitForAllWorkersToComplete();
    static void SignalWorkerCompletion();
    static void ResetSynchronization();
    
    // Safe ROOT file operations
    bool SafeWriteRootFile();
    bool ValidateRootFile(const G4String& filename);
    void CleanupRootObjects();
    
    // Auto-save mechanism
    void EnableAutoSave(G4int interval = 1000);
    void DisableAutoSave();
    void PerformAutoSave();
    
    // Variables for the branch (edep [MeV], positions [mm])
    void SetEventData(G4double edep, G4double x, G4double y, G4double z);
    
    // Method to set initial particle gun position [mm]
    void SetInitialPos(G4double x, G4double y, G4double z);
    
    // Method to set nearest pixel position [mm]
    void SetNearestPixelPos(G4double x, G4double y);
    
    // Method to set initial particle energy [MeV]
    void SetInitialEnergy(G4double energy);
    
    // Method to set pixel hit status
    void SetPixelHitStatus(G4bool isPixelHit);
    
    // Method to set pixel classification data (hit status and delta values)
    void SetPixelClassification(G4bool isWithinD0, G4double pixelTrueDeltaX, G4double pixelTrueDeltaY);
    
    // Method to set neighborhood (9x9) grid angle data for non-pixel hits
    void SetNeighborhoodGridData(const std::vector<G4double>& angles);
    
    // Method to set neighborhood (9x9) grid charge sharing data for non-pixel hits
    void SetNeighborhoodChargeData(const std::vector<G4double>& chargeFractions,
                          const std::vector<G4double>& distances,
                          const std::vector<G4double>& chargeValues,
                          const std::vector<G4double>& chargeCoulombs);
    
    // Method to set detector grid parameters for saving to ROOT
    void SetDetectorGridParameters(G4double pixelSize, G4double pixelSpacing, 
                                   G4double pixelCornerOffset, G4double detSize, 
                                   G4int numBlocksPerSide);
    
    // Method to set 2D Gauss fit results (central row and column fitting)
    void Set2DGaussResults(G4double x_center, G4double x_sigma, G4double x_amp,
                                G4double x_center_err, G4double x_sigma_err, G4double x_amp_err,
                                G4double x_vert_offset, G4double x_vert_offset_err,
                                G4double x_chi2red, G4double x_pp, G4int x_dof,
                                G4double y_center, G4double y_sigma, G4double y_amp,
                                G4double y_center_err, G4double y_sigma_err, G4double y_amp_err,
                                G4double y_vert_offset, G4double y_vert_offset_err,
                                G4double y_chi2red, G4double y_pp, G4int y_dof,
                                G4double x_charge_err, G4double y_charge_err,
                                G4bool fit_success);
    
    // Method to set diagonal Gauss fit results (4 separate fits: Main X, Main Y, Sec X, Sec Y)
    void SetDiagGaussResults(G4double main_diag_x_center, G4double main_diag_x_sigma, G4double main_diag_x_amp,
                                      G4double main_diag_x_center_err, G4double main_diag_x_sigma_err, G4double main_diag_x_amp_err,
                                      G4double main_diag_x_vert_offset, G4double main_diag_x_vert_offset_err,
                                      G4double main_diag_x_chi2red, G4double main_diag_x_pp, G4int main_diag_x_dof, G4bool main_diag_x_fit_success,
                                      G4double main_diag_y_center, G4double main_diag_y_sigma, G4double main_diag_y_amp,
                                      G4double main_diag_y_center_err, G4double main_diag_y_sigma_err, G4double main_diag_y_amp_err,
                                      G4double main_diag_y_vert_offset, G4double main_diag_y_vert_offset_err,
                                      G4double main_diag_y_chi2red, G4double main_diag_y_pp, G4int main_diag_y_dof, G4bool main_diag_y_fit_success,
                                      G4double sec_diag_x_center, G4double sec_diag_x_sigma, G4double sec_diag_x_amp,
                                      G4double sec_diag_x_center_err, G4double sec_diag_x_sigma_err, G4double sec_diag_x_amp_err,
                                      G4double sec_diag_x_vert_offset, G4double sec_diag_x_vert_offset_err,
                                      G4double sec_diag_x_chi2red, G4double sec_diag_x_pp, G4int sec_diag_x_dof, G4bool sec_diag_x_fit_success,
                                      G4double sec_diag_y_center, G4double sec_diag_y_sigma, G4double sec_diag_y_amp,
                                      G4double sec_diag_y_center_err, G4double sec_diag_y_sigma_err, G4double sec_diag_y_amp_err,
                                      G4double sec_diag_y_vert_offset, G4double sec_diag_y_vert_offset_err,
                                      G4double sec_diag_y_chi2red, G4double sec_diag_y_pp, G4int sec_diag_y_dof, G4bool sec_diag_y_fit_success,
                                      G4bool fit_success);
    
    // Method to set 2D Lorentz fit results (central row and column fitting)
    void Set2DLorentzResults(G4double x_center, G4double x_gamma, G4double x_amp,
                                  G4double x_center_err, G4double x_gamma_err, G4double x_amp_err,
                                  G4double x_vert_offset, G4double x_vert_offset_err,
                                  G4double x_chi2red, G4double x_pp, G4int x_dof,
                                  G4double y_center, G4double y_gamma, G4double y_amp,
                                  G4double y_center_err, G4double y_gamma_err, G4double y_amp_err,
                                  G4double y_vert_offset, G4double y_vert_offset_err,
                                  G4double y_chi2red, G4double y_pp, G4int y_dof,
                                  G4double x_charge_err, G4double y_charge_err,
                                  G4bool fit_success);
    

    
    // Method to set diagonal Lorentz fit results (4 separate fits: Main X, Main Y, Sec X, Sec Y)
    void SetDiagLorentzResults(G4double main_diag_x_center, G4double main_diag_x_gamma, G4double main_diag_x_amp,
                                        G4double main_diag_x_center_err, G4double main_diag_x_gamma_err, G4double main_diag_x_amp_err,
                                        G4double main_diag_x_vert_offset, G4double main_diag_x_vert_offset_err,
                                        G4double main_diag_x_chi2red, G4double main_diag_x_pp, G4int main_diag_x_dof, G4bool main_diag_x_fit_success,
                                        G4double main_diag_y_center, G4double main_diag_y_gamma, G4double main_diag_y_amp,
                                        G4double main_diag_y_center_err, G4double main_diag_y_gamma_err, G4double main_diag_y_amp_err,
                                        G4double main_diag_y_vert_offset, G4double main_diag_y_vert_offset_err,
                                        G4double main_diag_y_chi2red, G4double main_diag_y_pp, G4int main_diag_y_dof, G4bool main_diag_y_fit_success,
                                        G4double sec_diag_x_center, G4double sec_diag_x_gamma, G4double sec_diag_x_amp,
                                        G4double sec_diag_x_center_err, G4double sec_diag_x_gamma_err, G4double sec_diag_x_amp_err,
                                        G4double sec_diag_x_vert_offset, G4double sec_diag_x_vert_offset_err,
                                        G4double sec_diag_x_chi2red, G4double sec_diag_x_pp, G4int sec_diag_x_dof, G4bool sec_diag_x_fit_success,
                                        G4double sec_diag_y_center, G4double sec_diag_y_gamma, G4double sec_diag_y_amp,
                                        G4double sec_diag_y_center_err, G4double sec_diag_y_gamma_err, G4double sec_diag_y_amp_err,
                                        G4double sec_diag_y_vert_offset, G4double sec_diag_y_vert_offset_err,
                                        G4double sec_diag_y_chi2red, G4double sec_diag_y_pp, G4int sec_diag_y_dof, G4bool sec_diag_y_fit_success,
                                        G4bool fit_success);

    // Method to set 2D Power-Law Lorentz fit results (central row and column fitting)
    // Model: y(x) = A / (1 + ((x-m)/gamma)^2)^beta + B
    void Set2DPowerLorentzResults(G4double x_center, G4double x_gamma, G4double x_beta, G4double x_amp,
                                        G4double x_center_err, G4double x_gamma_err, G4double x_beta_err, G4double x_amp_err,
                                        G4double x_vert_offset, G4double x_vert_offset_err,
                                        G4double x_chi2red, G4double x_pp, G4int x_dof,
                                        G4double y_center, G4double y_gamma, G4double y_beta, G4double y_amp,
                                        G4double y_center_err, G4double y_gamma_err, G4double y_beta_err, G4double y_amp_err,
                                        G4double y_vert_offset, G4double y_vert_offset_err,
                                        G4double y_chi2red, G4double y_pp, G4int y_dof,
                                        G4double x_charge_err, G4double y_charge_err,
                                        G4bool fit_success);
    
    // Method to set diagonal Power-Law Lorentz fit results (4 separate fits: Main X, Main Y, Sec X, Sec Y)
    // Model: y(x) = A / (1 + ((x-m)/gamma)^2)^beta + B
    void SetDiagPowerLorentzResults(G4double main_diag_x_center, G4double main_diag_x_gamma, G4double main_diag_x_beta, G4double main_diag_x_amp,
                                              G4double main_diag_x_center_err, G4double main_diag_x_gamma_err, G4double main_diag_x_beta_err, G4double main_diag_x_amp_err,
                                              G4double main_diag_x_vert_offset, G4double main_diag_x_vert_offset_err,
                                              G4double main_diag_x_chi2red, G4double main_diag_x_pp, G4int main_diag_x_dof, G4bool main_diag_x_fit_success,
                                              G4double main_diag_y_center, G4double main_diag_y_gamma, G4double main_diag_y_beta, G4double main_diag_y_amp,
                                              G4double main_diag_y_center_err, G4double main_diag_y_gamma_err, G4double main_diag_y_beta_err, G4double main_diag_y_amp_err,
                                              G4double main_diag_y_vert_offset, G4double main_diag_y_vert_offset_err,
                                              G4double main_diag_y_chi2red, G4double main_diag_y_pp, G4int main_diag_y_dof, G4bool main_diag_y_fit_success,
                                              G4double sec_diag_x_center, G4double sec_diag_x_gamma, G4double sec_diag_x_beta, G4double sec_diag_x_amp,
                                              G4double sec_diag_x_center_err, G4double sec_diag_x_gamma_err, G4double sec_diag_x_beta_err, G4double sec_diag_x_amp_err,
                                              G4double sec_diag_x_vert_offset, G4double sec_diag_x_vert_offset_err,
                                              G4double sec_diag_x_chi2red, G4double sec_diag_x_pp, G4int sec_diag_x_dof, G4bool sec_diag_x_fit_success,
                                              G4double sec_diag_y_center, G4double sec_diag_y_gamma, G4double sec_diag_y_beta, G4double sec_diag_y_amp,
                                              G4double sec_diag_y_center_err, G4double sec_diag_y_gamma_err, G4double sec_diag_y_beta_err, G4double sec_diag_y_amp_err,
                                              G4double sec_diag_y_vert_offset, G4double sec_diag_y_vert_offset_err,
                                              G4double sec_diag_y_chi2red, G4double sec_diag_y_pp, G4int sec_diag_y_dof, G4bool sec_diag_y_fit_success,
                                              G4bool fit_success);

    // =============================================
    // 3D FIT RESULTS SETTER METHODS
    // =============================================
    
    // Method to set 3D Lorentz fit results (entire neighborhood surface fitting)
    // Model: z(x,y) = A / (1 + ((x-mx)/γx)^2 + ((y-my)/γy)^2) + B
    void Set3DLorentzResults(G4double center_x, G4double center_y, G4double gamma_x, G4double gamma_y, G4double amp, G4double vert_offset,
                                   G4double center_x_err, G4double center_y_err, G4double gamma_x_err, G4double gamma_y_err, G4double amp_err, G4double vert_offset_err,
                                   G4double chi2red, G4double pp, G4int dof,
                                   G4double charge_err,
                                   G4bool fit_success);
    
    // Method to set 3D Gauss fit results (entire neighborhood surface fitting)
    // Model: z(x,y) = A * exp(-((x-mx)^2/(2*σx^2) + (y-my)^2/(2*σy^2))) + B
    void Set3DGaussResults(G4double center_x, G4double center_y, G4double sigma_x, G4double sigma_y, G4double amp, G4double vert_offset,
                                 G4double center_x_err, G4double center_y_err, G4double sigma_x_err, G4double sigma_y_err, G4double amp_err, G4double vert_offset_err,
                                 G4double chi2red, G4double pp, G4int dof,
                                 G4double charge_err,
                                 G4bool fit_success);
    
    // Method to set 3D Power-Law Lorentz fit results (entire neighborhood surface fitting)
    // Model: z(x,y) = A / (1 + ((x-mx)/γx)^2 + ((y-my)/γy)^2)^β + B
    void Set3DPowerLorentzResults(G4double center_x, G4double center_y, G4double gamma_x, G4double gamma_y, G4double beta, G4double amp, G4double vert_offset,
                                        G4double center_x_err, G4double center_y_err, G4double gamma_x_err, G4double gamma_y_err, G4double beta_err, G4double amp_err, G4double vert_offset_err,
                                        G4double chi2red, G4double pp, G4int dof,
                                        G4double charge_err,
                                        G4bool fit_success);
    
    // Fill the ROOT tree with current event data
    void FillTree();
    
    // Method to store automatic radius selection results
    void SetAutoRadiusResults(G4int selectedRadius);
    
    // Method to set scorer data from Multi-Functional Detector
    void SetScorerData(G4double energyDeposit, G4int hitCount, G4bool dataValid);
    
    // Method to validate scorer data before tree storage
    void ValidateScorerDataForTreeStorage();
    
    // Method to verify scorer data is written to ROOT tree
    void VerifyScorerDataInTree();
    
    // Method to set hit purity tracking data from EventAction
    void SetHitPurityData(G4bool pureSiliconHit, G4bool aluminumContaminated, G4bool chargeCalculationEnabled);
 
private:
    // =============================================
    // COORDINATE TRANSFORMATION HELPER METHODS
    // =============================================
    
    // Apply rotation matrix transformation for diagonal coordinates
    void TransformDiagCoords(G4double x_prime, G4double y_prime, G4double theta_deg, 
                                      G4double& x_transformed, G4double& y_transformed);
    
    // Calc transformed coordinates for all diagonal fits
    void CalcTransformedDiagCoords();
    
    // Calc mean estimations from all fitting methods
    void CalcMeanEstimations();
    
    // Helper functions to organize branch creation
    void CreateHitsBranches();
    void CreateGaussBranches();
    void CreateLorentzBranches();
    void CreatePowerLorentzBranches();
    void Create3DBranches();
    void CreateNeighborhoodBranches();
    void CreateMetadataBranches();

    TFile* fRootFile;
    TTree* fTree;
    
    // Thread-safety mutex for ROOT operations
    static std::mutex fRootMutex;
    
    // Thread synchronization for robust file operations
    static std::atomic<int> fWorkersCompleted;
    static std::atomic<int> fTotalWorkers;
    static std::condition_variable fWorkerCompletionCV;
    static std::mutex fSyncMutex;
    static std::atomic<bool> fAllWorkersCompleted;
    
    // Auto-save mechanism
    G4bool fAutoSaveEnabled;
    G4int fAutoSaveInterval;
    G4int fEventsSinceLastSave;
    
    // Initial particle gun position
    G4double fInitialX, fInitialY, fInitialZ;
    
    // Nearest pixel position
    G4double fNearestPixelX, fNearestPixelY;

    // =============================================
    // HITS DATA VARIABLES
    // =============================================
    G4double fTrueX;   // True Hit pos X [mm]
    G4double fTrueY;   // True Hit pos Y [mm]
    G4double fInitX;  // Initial X [mm]
    G4double fInitY;  // Initial Y [mm]
    G4double fInitZ;  // Initial Z [mm]
    G4double fPixelX; // Nearest to hit pixel center X [mm]
    G4double fPixelY; // Nearest to hit pixel center Y [mm]
    G4double fEdep;   // Energy depositionit [MeV]
    G4double fPixelTrueDeltaX; // Delta X from pixel center to true pos [mm] (x_pixel - x_true)
    G4double fPixelTrueDeltaY; // Delta Y from pixel center to true pos [mm] (y_pixel - y_true)
    
    // Delta variables for estimations vs true pos
    G4double fGaussRowDeltaX;
    G4double fGaussColDeltaY;
    G4double fLorentzRowDeltaX;
    G4double fLorentzColDeltaY;
    G4double fPowerLorentzRowDeltaX;
    G4double fPowerLorentzColDeltaY;
    
    // 3D fitting delta variables
    G4double f3DLorentzDeltaX;          // Delta X from 3D Lorentz fit to True Pos [mm]
    G4double f3DLorentzDeltaY;          // Delta Y from 3D Lorentz fit to True Pos [mm]
    G4double f3DGaussDeltaX;            // Delta X from 3D Gauss fit to True Pos [mm]
    G4double f3DGaussDeltaY;            // Delta Y from 3D Gauss fit to True Pos [mm]
    G4double f3DPowerLorentzDeltaX;     // Delta X from 3D Power-Law Lorentz fit to True Pos [mm]
    G4double f3DPowerLorentzDeltaY;     // Delta Y from 3D Power-Law Lorentz fit to True Pos [mm]

    // =============================================
    // TRANSFORMED DIAG COORDINATES (ROTATION MATRIX)
    // =============================================
    
    // Transformed main diagonal coordinates (θ = 45°)
    G4double fGaussMainDiagTransformedX;      // Transformed X from main diagonal (x' -> x)
    G4double fGaussMainDiagTransformedY;      // Transformed Y from main diagonal (y' -> y)
    G4double fLorentzMainDiagTransformedX;    // Transformed X from main diagonal (x' -> x)
    G4double fLorentzMainDiagTransformedY;    // Transformed Y from main diagonal (y' -> y)
    
    // Transformed secondary diagonal coordinates (θ = -45°)
    G4double fGaussSecDiagTransformedX;    // Transformed X from secondary diagonal (x' -> x)
    G4double fGaussSecDiagTransformedY;    // Transformed Y from secondary diagonal (y' -> y)
    G4double fLorentzSecDiagTransformedX;  // Transformed X from secondary diagonal (x' -> x)
    G4double fLorentzSecDiagTransformedY;  // Transformed Y from secondary diagonal (y' -> y)
    G4double fPowerLorentzMainDiagTransformedX;    // Transformed X from Power-Law Lorentz main diagonal (x' -> x)
    G4double fPowerLorentzMainDiagTransformedY;    // Transformed Y from Power-Law Lorentz main diagonal (y' -> y)
    G4double fPowerLorentzSecDiagTransformedX;  // Transformed X from Power-Law Lorentz secondary diagonal (x' -> x)
    G4double fPowerLorentzSecDiagTransformedY;  // Transformed Y from Power-Law Lorentz secondary diagonal (y' -> y)
    
    // Delta values for transformed coordinates vs true pos
    G4double fGaussMainDiagTransformedDeltaX;   // x_transformed - x_true (main diagonal)
    G4double fGaussMainDiagTransformedDeltaY;   // y_transformed - y_true (main diagonal)
    G4double fGaussSecDiagTransformedDeltaX; // x_transformed - x_true (secondary diagonal)
    G4double fGaussSecDiagTransformedDeltaY; // y_transformed - y_true (secondary diagonal)
    G4double fLorentzMainDiagTransformedDeltaX;   // x_transformed - x_true (main diagonal)
    G4double fLorentzMainDiagTransformedDeltaY;   // y_transformed - y_true (main diagonal)
    G4double fLorentzSecDiagTransformedDeltaX; // x_transformed - x_true (secondary diagonal)
    G4double fLorentzSecDiagTransformedDeltaY; // y_transformed - y_true (secondary diagonal)
    G4double fPowerLorentzMainDiagTransformedDeltaX;   // x_transformed - x_true (Power-Law Lorentz main diagonal)
    G4double fPowerLorentzMainDiagTransformedDeltaY;   // y_transformed - y_true (Power-Law Lorentz main diagonal)
    G4double fPowerLorentzSecDiagTransformedDeltaX; // x_transformed - x_true (Power-Law Lorentz secondary diagonal)
    G4double fPowerLorentzSecDiagTransformedDeltaY; // y_transformed - y_true (Power-Law Lorentz secondary diagonal)

    // =============================================
    // MEAN ESTIMATIONS FROM ALL FIT METHODS
    // =============================================
    
    // Mean of all X coordinate estimations (row, transformed diagonals)
    G4double fGaussMeanTrueDeltaX;   // Mean delta X from all Gauss estimation methods to True Pos [mm]
    G4double fGaussMeanTrueDeltaY;   // Mean delta Y from all Gauss estimation methods to True Pos [mm]
    G4double fLorentzMeanTrueDeltaX; // Mean delta X from all Lorentz estimation methods to True Pos [mm]
    G4double fLorentzMeanTrueDeltaY; // Mean delta Y from all Lorentz estimation methods to True Pos [mm]
    G4double fPowerLorentzMeanTrueDeltaX; // Mean delta X from all Power-Law Lorentz estimation methods to True Pos [mm]
    G4double fPowerLorentzMeanTrueDeltaY; // Mean delta Y from all Power-Law Lorentz estimation methods to True Pos [mm]

    // =============================================
    // AUTOMATIC RADIUS SELECTION VARIABLES
    // =============================================
    
    G4int fSelectedRadius;          // Automatically selected radius for this event

    // =============================================
    // GAUSS FITS VARIABLES
    // =============================================
    
    // GaussRow/GaussRowX
    G4double fGaussRowAmp;
    G4double fGaussRowAmpErr;
    G4double fGaussRowSigma;
    G4double fGaussRowSigmaErr;
    G4double fGaussRowVertOffset;
    G4double fGaussRowVertOffsetErr;
    G4double fGaussRowCenter;
    G4double fGaussRowCenterErr;
    G4double fGaussRowChi2red;
    G4double fGaussRowPp;
    G4int fGaussRowDOF;
    
    // Charge uncertainties (5% of max charge for Gauss fits)
    G4double fGaussRowChargeErr;     // Row charge uncertainty (5% of max charge)
    
    // GaussCol/GaussColY
    G4double fGaussColAmp;
    G4double fGaussColAmpErr;
    G4double fGaussColSigma;
    G4double fGaussColSigmaErr;
    G4double fGaussColVertOffset;
    G4double fGaussColVertOffsetErr;
    G4double fGaussColCenter;
    G4double fGaussColCenterErr;
    G4double fGaussColChi2red;
    G4double fGaussColPp;
    G4int fGaussColDOF;
    
    // Charge err for Gauss column fit
    G4double fGaussColChargeErr;  // Col charge uncertainty (5% of max charge)
    
    // GaussMainDiag/GaussMainDiagX
    G4double fGaussMainDiagXAmp;
    G4double fGaussMainDiagXAmpErr;
    G4double fGaussMainDiagXSigma;
    G4double fGaussMainDiagXSigmaErr;
    G4double fGaussMainDiagXVertOffset;
    G4double fGaussMainDiagXVertOffsetErr;
    G4double fGaussMainDiagXCenter;
    G4double fGaussMainDiagXCenterErr;
    G4double fGaussMainDiagXChi2red;
    G4double fGaussMainDiagXPp;
    G4int fGaussMainDiagXDOF;
    
    // GaussMainDiag/GaussMainDiagY
    G4double fGaussMainDiagYAmp;
    G4double fGaussMainDiagYAmpErr;
    G4double fGaussMainDiagYSigma;
    G4double fGaussMainDiagYSigmaErr;
    G4double fGaussMainDiagYVertOffset;
    G4double fGaussMainDiagYVertOffsetErr;
    G4double fGaussMainDiagYCenter;
    G4double fGaussMainDiagYCenterErr;
    G4double fGaussMainDiagYChi2red;
    G4double fGaussMainDiagYPp;
    G4int fGaussMainDiagYDOF;
    
    // GaussSecDiag/GaussSecDiagX
    G4double fGaussSecDiagXAmp;
    G4double fGaussSecDiagXAmpErr;
    G4double fGaussSecDiagXSigma;
    G4double fGaussSecDiagXSigmaErr;
    G4double fGaussSecDiagXVertOffset;
    G4double fGaussSecDiagXVertOffsetErr;
    G4double fGaussSecDiagXCenter;
    G4double fGaussSecDiagXCenterErr;
    G4double fGaussSecDiagXChi2red;
    G4double fGaussSecDiagXPp;
    G4int fGaussSecDiagXDOF;
    
    // GaussSecDiag/GaussSecDiagY
    G4double fGaussSecDiagYAmp;
    G4double fGaussSecDiagYAmpErr;
    G4double fGaussSecDiagYSigma;
    G4double fGaussSecDiagYSigmaErr;
    G4double fGaussSecDiagYVertOffset;
    G4double fGaussSecDiagYVertOffsetErr;
    G4double fGaussSecDiagYCenter;
    G4double fGaussSecDiagYCenterErr;
    G4double fGaussSecDiagYChi2red;
    G4double fGaussSecDiagYPp;
    G4int fGaussSecDiagYDOF;

    // =============================================
    // LORENTZ FITS VARIABLES
    // =============================================
    
    // LorentzRow/LorentzRowX
    G4double fLorentzRowAmp;
    G4double fLorentzRowAmpErr;
    G4double fLorentzRowGamma;
    G4double fLorentzRowGammaErr;
    G4double fLorentzRowVertOffset;
    G4double fLorentzRowVertOffsetErr;
    G4double fLorentzRowCenter;
    G4double fLorentzRowCenterErr;
    G4double fLorentzRowChi2red;
    G4double fLorentzRowPp;
    G4int fLorentzRowDOF;
    
    // Charge err for Lorentz row fit  
    G4double fLorentzRowChargeErr;   // Row charge uncertainty (5% of max charge)
    
    // LorentzCol/LorentzColY
    G4double fLorentzColAmp;
    G4double fLorentzColAmpErr;
    G4double fLorentzColGamma;
    G4double fLorentzColGammaErr;
    G4double fLorentzColVertOffset;
    G4double fLorentzColVertOffsetErr;
    G4double fLorentzColCenter;
    G4double fLorentzColCenterErr;
    G4double fLorentzColChi2red;
    G4double fLorentzColPp;
    G4int fLorentzColDOF;
    
    // Charge err for Lorentz column fit
    G4double fLorentzColChargeErr; // Column charge uncertainty (5% of max charge)
    
    // LorentzMainDiag/LorentzMainDiagX
    G4double fLorentzMainDiagXAmp;
    G4double fLorentzMainDiagXAmpErr;
    G4double fLorentzMainDiagXGamma;
    G4double fLorentzMainDiagXGammaErr;
    G4double fLorentzMainDiagXVertOffset;
    G4double fLorentzMainDiagXVertOffsetErr;
    G4double fLorentzMainDiagXCenter;
    G4double fLorentzMainDiagXCenterErr;
    G4double fLorentzMainDiagXChi2red;
    G4double fLorentzMainDiagXPp;
    G4int fLorentzMainDiagXDOF;
    
    // LorentzMainDiag/LorentzMainDiagY
    G4double fLorentzMainDiagYAmp;
    G4double fLorentzMainDiagYAmpErr;
    G4double fLorentzMainDiagYGamma;
    G4double fLorentzMainDiagYGammaErr;
    G4double fLorentzMainDiagYVertOffset;
    G4double fLorentzMainDiagYVertOffsetErr;
    G4double fLorentzMainDiagYCenter;
    G4double fLorentzMainDiagYCenterErr;
    G4double fLorentzMainDiagYChi2red;
    G4double fLorentzMainDiagYPp;
    G4int fLorentzMainDiagYDOF;
    
    // LorentzSecDiag/LorentzSecDiagX
    G4double fLorentzSecDiagXAmp;
    G4double fLorentzSecDiagXAmpErr;
    G4double fLorentzSecDiagXGamma;
    G4double fLorentzSecDiagXGammaErr;
    G4double fLorentzSecDiagXVertOffset;
    G4double fLorentzSecDiagXVertOffsetErr;
    G4double fLorentzSecDiagXCenter;
    G4double fLorentzSecDiagXCenterErr;
    G4double fLorentzSecDiagXChi2red;
    G4double fLorentzSecDiagXPp;
    G4int fLorentzSecDiagXDOF;
    
    // LorentzSecDiag/LorentzSecDiagY
    G4double fLorentzSecDiagYAmp;
    G4double fLorentzSecDiagYAmpErr;
    G4double fLorentzSecDiagYGamma;
    G4double fLorentzSecDiagYGammaErr;
    G4double fLorentzSecDiagYVertOffset;
    G4double fLorentzSecDiagYVertOffsetErr;
    G4double fLorentzSecDiagYCenter;
    G4double fLorentzSecDiagYCenterErr;
    G4double fLorentzSecDiagYChi2red;
    G4double fLorentzSecDiagYPp;
    G4int fLorentzSecDiagYDOF;

    // =============================================
    // POWER-LAW LORENTZ FITS VARIABLES
    // =============================================
    
    // PowerLorentzRow/PowerLorentzRowX
    G4double fPowerLorentzRowAmp;
    G4double fPowerLorentzRowAmpErr;
    G4double fPowerLorentzRowBeta;
    G4double fPowerLorentzRowBetaErr;
    G4double fPowerLorentzRowGamma;
    G4double fPowerLorentzRowGammaErr;
    G4double fPowerLorentzRowVertOffset;
    G4double fPowerLorentzRowVertOffsetErr;
    G4double fPowerLorentzRowCenter;
    G4double fPowerLorentzRowCenterErr;
    G4double fPowerLorentzRowChi2red;
    G4double fPowerLorentzRowPp;
    G4int fPowerLorentzRowDOF;
    
    // PowerLorentzCol/PowerLorentzColY
    G4double fPowerLorentzColAmp;
    G4double fPowerLorentzColAmpErr;
    G4double fPowerLorentzColBeta;
    G4double fPowerLorentzColBetaErr;
    G4double fPowerLorentzColGamma;
    G4double fPowerLorentzColGammaErr;
    G4double fPowerLorentzColVertOffset;
    G4double fPowerLorentzColVertOffsetErr;
    G4double fPowerLorentzColCenter;
    G4double fPowerLorentzColCenterErr;
    G4double fPowerLorentzColChi2red;
    G4double fPowerLorentzColPp;
    G4int fPowerLorentzColDOF;
    
    // Charge uncertainties for Power-Law Lorentz fits (5% of max charge)
    G4double fPowerLorentzRowChargeErr;    // Row charge uncertainty (5% of max charge)
    G4double fPowerLorentzColChargeErr; // Column charge uncertainty (5% of max charge)
    
    // PowerLorentzMainDiag/PowerLorentzMainDiagX
    G4double fPowerLorentzMainDiagXAmp;
    G4double fPowerLorentzMainDiagXAmpErr;
    G4double fPowerLorentzMainDiagXBeta;
    G4double fPowerLorentzMainDiagXBetaErr;
    G4double fPowerLorentzMainDiagXGamma;
    G4double fPowerLorentzMainDiagXGammaErr;
    G4double fPowerLorentzMainDiagXVertOffset;
    G4double fPowerLorentzMainDiagXVertOffsetErr;
    G4double fPowerLorentzMainDiagXCenter;
    G4double fPowerLorentzMainDiagXCenterErr;
    G4double fPowerLorentzMainDiagXChi2red;
    G4double fPowerLorentzMainDiagXPp;
    G4int fPowerLorentzMainDiagXDOF;
    
    // PowerLorentzMainDiag/PowerLorentzMainDiagY
    G4double fPowerLorentzMainDiagYAmp;
    G4double fPowerLorentzMainDiagYAmpErr;
    G4double fPowerLorentzMainDiagYBeta;
    G4double fPowerLorentzMainDiagYBetaErr;
    G4double fPowerLorentzMainDiagYGamma;
    G4double fPowerLorentzMainDiagYGammaErr;
    G4double fPowerLorentzMainDiagYVertOffset;
    G4double fPowerLorentzMainDiagYVertOffsetErr;
    G4double fPowerLorentzMainDiagYCenter;
    G4double fPowerLorentzMainDiagYCenterErr;
    G4double fPowerLorentzMainDiagYChi2red;
    G4double fPowerLorentzMainDiagYPp;
    G4int fPowerLorentzMainDiagYDOF;
    
    // PowerLorentzSecDiag/PowerLorentzSecDiagX
    G4double fPowerLorentzSecDiagXAmp;
    G4double fPowerLorentzSecDiagXAmpErr;
    G4double fPowerLorentzSecDiagXBeta;
    G4double fPowerLorentzSecDiagXBetaErr;
    G4double fPowerLorentzSecDiagXGamma;
    G4double fPowerLorentzSecDiagXGammaErr;
    G4double fPowerLorentzSecDiagXVertOffset;
    G4double fPowerLorentzSecDiagXVertOffsetErr;
    G4double fPowerLorentzSecDiagXCenter;
    G4double fPowerLorentzSecDiagXCenterErr;
    G4double fPowerLorentzSecDiagXChi2red;
    G4double fPowerLorentzSecDiagXPp;
    G4int fPowerLorentzSecDiagXDOF;
    
    // PowerLorentzSecDiag/PowerLorentzSecDiagY
    G4double fPowerLorentzSecDiagYAmp;
    G4double fPowerLorentzSecDiagYAmpErr;
    G4double fPowerLorentzSecDiagYBeta;
    G4double fPowerLorentzSecDiagYBetaErr;
    G4double fPowerLorentzSecDiagYGamma;
    G4double fPowerLorentzSecDiagYGammaErr;
    G4double fPowerLorentzSecDiagYVertOffset;
    G4double fPowerLorentzSecDiagYVertOffsetErr;
    G4double fPowerLorentzSecDiagYCenter;
    G4double fPowerLorentzSecDiagYCenterErr;
    G4double fPowerLorentzSecDiagYChi2red;
    G4double fPowerLorentzSecDiagYPp;
    G4int fPowerLorentzSecDiagYDOF;

    // =============================================
    // 3D LORENTZ FITS VARIABLES
    // =============================================
    
    // 3D Lorentz fit parameters: z(x,y) = A / (1 + ((x-mx)/γx)^2 + ((y-my)/γy)^2) + B
    G4double f3DLorentzCenterX;            // mx parameter (X center)
    G4double f3DLorentzCenterY;            // my parameter (Y center)
    G4double f3DLorentzGammaX;             // γx parameter (X width/HWHM)
    G4double f3DLorentzGammaY;             // γy parameter (Y width/HWHM)
    G4double f3DLorentzAmp;          // A parameter (peak amplitude)
    G4double f3DLorentzVertOffset;     // B parameter (baseline)
    
    // 3D Lorentz fit parameter errors
    G4double f3DLorentzCenterXErr;         
    G4double f3DLorentzCenterYErr;         
    G4double f3DLorentzGammaXErr;          
    G4double f3DLorentzGammaYErr;          
    G4double f3DLorentzAmpErr;       
    G4double f3DLorentzVertOffsetErr;  
    
    // 3D Lorentz fit statistics
    G4double f3DLorentzChi2red;            // Reduced Chi-squared
    G4double f3DLorentzPp;                 // P-value  
    G4int f3DLorentzDOF;                   // Degrees of Freedom
    G4double f3DLorentzChargeErr;  // Charge uncertainty (5% of max charge)
    G4bool f3DLorentzSuccess;           //  success flag

    // =============================================
    // 3D GAUSS FITS VARIABLES
    // =============================================
    
    // 3D Gauss fit parameters: z(x,y) = A * exp(-((x-mx)^2/(2*σx^2) + (y-my)^2/(2*σy^2))) + B
    G4double f3DGaussCenterX;            // mx parameter (X center)
    G4double f3DGaussCenterY;            // my parameter (Y center)
    G4double f3DGaussSigmaX;             // σx parameter (X standard deviation)
    G4double f3DGaussSigmaY;             // σy parameter (Y standard deviation)
    G4double f3DGaussAmp;          // A parameter (peak amplitude)
    G4double f3DGaussVertOffset;     // B parameter (baseline)
    
    // 3D Gauss fit parameter errors
    G4double f3DGaussCenterXErr;         
    G4double f3DGaussCenterYErr;         
    G4double f3DGaussSigmaXErr;          
    G4double f3DGaussSigmaYErr;          
    G4double f3DGaussAmpErr;       
    G4double f3DGaussVertOffsetErr;  
    
    // 3D Gauss fit statistics
    G4double f3DGaussChi2red;            // Reduced Chi-squared
    G4double f3DGaussPp;                 // P-value  
    G4int f3DGaussDOF;                   // Degrees of Freedom
    G4double f3DGaussChargeErr;  // Charge uncertainty (5% of max charge)
    G4bool f3DGaussSuccess;           //  success flag

    // =============================================
    // 3D POWER-LAW LORENTZ FITS VARIABLES
    // =============================================
    
    // 3D Power-Law Lorentz fit parameters: z(x,y) = A / (1 + ((x-mx)/γx)^2 + ((y-my)/γy)^2)^β + B
    G4double f3DPowerLorentzCenterX;            // mx parameter (X center)
    G4double f3DPowerLorentzCenterY;            // my parameter (Y center)
    G4double f3DPowerLorentzGammaX;             // γx parameter (X width)
    G4double f3DPowerLorentzGammaY;             // γy parameter (Y width)
    G4double f3DPowerLorentzBeta;               // β parameter (power exponent)  
    G4double f3DPowerLorentzAmp;          // A parameter (peak amplitude)
    G4double f3DPowerLorentzVertOffset;     // B parameter (baseline)
    
    // 3D Power-Law Lorentz fit parameter errors
    G4double f3DPowerLorentzCenterXErr;         
    G4double f3DPowerLorentzCenterYErr;         
    G4double f3DPowerLorentzGammaXErr;          
    G4double f3DPowerLorentzGammaYErr;          
    G4double f3DPowerLorentzBetaErr;            
    G4double f3DPowerLorentzAmpErr;       
    G4double f3DPowerLorentzVertOffsetErr;  
    
    // 3D Power-Law Lorentz fit statistics
    G4double f3DPowerLorentzChi2red;            // Reduced Chi-squared
    G4double f3DPowerLorentzPp;                 // P-value  
    G4int f3DPowerLorentzDOF;                   // Degrees of Freedom
    G4double f3DPowerLorentzChargeErr;  // Charge uncertainty (5% of max charge)
    G4bool f3DPowerLorentzSuccess;           //  success flag

    // Legacy variables that may still be used
    G4bool fIsPixelHit;  // True if hit is on pixel OR distance <= D0
    
    // NON-PIXEL HIT DATA (distance > D0 and not on pixel)
    std::vector<G4double> fNeighborhoodAngles; // Angles from hit to neighborhood grid pixels [deg]
    std::vector<G4double> fNeighborhoodChargeFractions; // Charge fractions for neighborhood grid pixels
    std::vector<G4double> fNeighborhoodDistances;         // Distances from hit to neighborhood grid pixels [mm]
    std::vector<G4double> fNeighborhoodCharge;       // Charge values in Coulombs for neighborhood grid pixels

    // Variables for particle information (reduced set)
    G4double fInitialEnergy;        // Initial particle energy [MeV]
    
    // Variables for detector grid parameters (stored as ROOT metadata)
    G4double fGridPixelSize;        // Pixel size [mm]
    G4double fGridPixelSpacing;     // Pixel spacing [mm]  
    G4double fGridPixelCornerOffset; // Pixel corner offset [mm]
    G4double fGridDetSize;          // Detector size [mm]
    G4int fGridNumBlocksPerSide;    // Number of blocks per side
    
    // Scorer data variables
    G4double fScorerEnergyDeposit;  // Energy deposit from Multi-Functional Detector [MeV]
    G4int fScorerHitCount;          // Hit count from Multi-Functional Detector
    G4bool fScorerDataValid;        // Validation flag for scorer data
    
    // Hit purity tracking variables for Multi-Functional Detector validation
    G4bool fPureSiliconHit;         // True if hit is purely in silicon (no aluminum contamination)
    G4bool fAluminumContaminated;   // True if hit has aluminum contamination
    G4bool fChargeCalculationEnabled; // True if charge sharing calculation was enabled
};

#endif